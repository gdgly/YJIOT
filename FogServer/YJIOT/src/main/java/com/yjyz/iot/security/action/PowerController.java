package com.yjyz.iot.security.action;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.yjyz.iot.comm.ConstParm;
import com.yjyz.iot.comm.RetInfoDto;
import com.yjyz.iot.security.dto.PowCommDto;
import com.yjyz.iot.security.entity.PowAppInfo;
import com.yjyz.iot.security.entity.PowResource;
import com.yjyz.iot.security.entity.PowRole;
import com.yjyz.iot.security.entity.PowRoleResourceKey;
import com.yjyz.iot.security.entity.PowSysUser;
import com.yjyz.iot.security.entity.PowUserRoleKey;
import com.yjyz.iot.security.service.IPowerService;
import com.yjyz.iot.security.utils.BASE64Util;
import com.yjyz.iot.security.utils.ClientJwtToken;
import com.yjyz.iot.security.utils.ClientJwtUtil;
import com.yjyz.iot.security.utils.MD5Util;

@RestController
@RequestMapping("/power")
public class PowerController {
	@Autowired
	private IPowerService powerService;
	@Autowired
	ClientJwtUtil jwtUtil;
	@Autowired
	MD5Util md5Util;
	@Autowired
	BASE64Util base64Util;

	private static Log log = LogFactory.getLog(PowerController.class);

	@Transactional
	@RequestMapping(value = "/login", method = RequestMethod.POST)
	public RetInfoDto login(@RequestBody PowSysUser dto) {
		RetInfoDto info = new RetInfoDto();
		try {

			List<PowAppInfo> appList = this.powerService.login(dto);
			if (appList.size() == 0) {
				info.meta.put("message", "Login fail. username or password is wrong!");
				info.meta.put("code", ConstParm.ERR_POW_LOGIN);
			} else {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("appList", appList);
			}
			return info;
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "Login fail. username or password is wrong!");
			info.meta.put("code", ConstParm.ERR_POW_LOGIN);
			return info;
		}
	}

	@Transactional
	@RequestMapping(value = "/getToken", method = RequestMethod.POST)
	public RetInfoDto getToken(@RequestBody PowCommDto dto) {
		RetInfoDto info = new RetInfoDto();

		try {
			PowSysUser sysUser = new PowSysUser();
			sysUser.setAppId(dto.getAppId());
			sysUser.setUsername(dto.getUserName());
			PowSysUser user = this.powerService.selSysUser(sysUser);
			ClientJwtToken jwtToken = new ClientJwtToken(user.getUsername(), dto.getAppId());
			String token = jwtUtil.createJWT(jwtToken);
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("token", token);
			return info;
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "get Token fail.");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
	}

	@Transactional
	@RequestMapping(value = "/addApp", method = RequestMethod.POST)
	public RetInfoDto addApp(@RequestBody PowAppInfo dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			PowAppInfo appInfo = this.powerService.addApp(dto);
			if (appInfo != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("appInfo", appInfo);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_APPINFO);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_APP_INFO);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delApp", method = RequestMethod.POST)
	public RetInfoDto delApp(@RequestBody PowAppInfo dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			boolean isOk = this.powerService.delApp(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_APPINFO);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_APP_INFO);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/updApp", method = RequestMethod.POST)
	public RetInfoDto updApp(@RequestBody PowAppInfo dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			boolean isOk = this.powerService.updApp(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_APPINFO);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_APP_INFO);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selApp", method = RequestMethod.POST)
	public RetInfoDto selApp(@RequestBody PowAppInfo dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			PowAppInfo appInfo = this.powerService.selApp(dto);
			info.meta.put("code", ConstParm.SUCESS_CODE);
			if (appInfo != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("appInfo", appInfo);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_APP_INFO);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selAppAll", method = RequestMethod.POST)
	public RetInfoDto selAppAll(@RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			List<PowAppInfo> appInfoList = this.powerService.selAppAll();
			info.meta.put("code", ConstParm.SUCESS_CODE);
			if (appInfoList.size() > 0) {
				info.data.put("appInfoList", appInfoList);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_APP_INFO);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/addRole", method = RequestMethod.POST)
	public RetInfoDto addRole(@RequestBody PowRole dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			PowRole role = this.powerService.addRole(dto);
			if (role != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("appInfo", role);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_ROLE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delRole", method = RequestMethod.POST)
	public RetInfoDto delRole(@RequestBody PowRole dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.delRole(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_ROLE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/updRole", method = RequestMethod.POST)
	public RetInfoDto updRole(@RequestBody PowRole dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.updRole(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_ROLE);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selRole", method = RequestMethod.POST)
	public RetInfoDto selRole(@RequestBody PowRole dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			PowRole role = this.powerService.selRole(dto);
			info.meta.put("code", ConstParm.SUCESS_CODE);
			if (role != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("role", role);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selRoleByApp", method = RequestMethod.POST)
	public RetInfoDto selRoleByApp(@RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			List<PowRole> roleList = this.powerService.selRoleByApp(jwtToken.getApp_id());
			info.meta.put("code", ConstParm.SUCESS_CODE);
			if (roleList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("roleList", roleList);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/addSysUser", method = RequestMethod.POST)
	public RetInfoDto addSysUser(@RequestBody PowSysUser dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.addSysUser(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_SYS_USER);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_SYS_USER);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delSysUser", method = RequestMethod.POST)
	public RetInfoDto delSysUser(@RequestBody PowSysUser dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.delSysUser(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_SYS_USER);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_SYS_USER);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/updSysUser", method = RequestMethod.POST)
	public RetInfoDto updSysUser(@RequestBody PowSysUser dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.updSysUser(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_SYS_USER);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_SYS_USER);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selSysUser", method = RequestMethod.POST)
	public RetInfoDto selSysUser(@RequestBody PowSysUser dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			PowSysUser sysUser = this.powerService.selSysUser(dto);
			if (sysUser != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("sysUser", sysUser);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_SYS_USER);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_SYS_USER);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selSysUserByApp", method = RequestMethod.POST)
	public RetInfoDto selSysUserByApp(@RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			List<PowSysUser> sysUserList = this.powerService.selSysUserByApp(jwtToken.getApp_id());
			if (sysUserList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("sysUserList", sysUserList);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_SYS_USER);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_SYS_USER);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/addResource", method = RequestMethod.POST)
	public RetInfoDto addResource(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			PowResource powResource = this.powerService.addResource(dto);
			if (powResource != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("resource", powResource);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delResource", method = RequestMethod.POST)
	public RetInfoDto delResource(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.delResource(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/updResource", method = RequestMethod.POST)
	public RetInfoDto updResource(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.updResource(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selResource", method = RequestMethod.POST)
	public RetInfoDto selResource(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			PowResource powResource = this.powerService.selResource(dto);
			if (powResource != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("resource", powResource);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selResourceByApp", method = RequestMethod.POST)
	public RetInfoDto selResourceByApp(@RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			List<PowResource> resourceList = new ArrayList<PowResource>();
			if (jwtToken.getUser_id().equals("admin")) {
				resourceList = this.powerService.selResourceByApp(jwtToken.getApp_id());
			} else {
				PowSysUser sysUser = new PowSysUser();
				sysUser.setUsername(jwtToken.getUser_id());
				sysUser.setAppId(jwtToken.getApp_id());
				resourceList = this.powerService.selUserResource(sysUser);
			}
			if (resourceList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("resourceList", resourceList);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/addRoleResource", method = RequestMethod.POST)
	public RetInfoDto addRoleResource(@RequestBody PowRoleResourceKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			boolean isOk = this.powerService.addRoleResource(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delRoleResource", method = RequestMethod.POST)
	public RetInfoDto delRoleResource(@RequestBody PowRoleResourceKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			boolean isOk = this.powerService.delRoleResource(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selRoleResource", method = RequestMethod.POST)
	public RetInfoDto selRoleResource(@RequestBody PowRole dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowResource> resourceList = this.powerService.selRoleResource(dto);
			if (resourceList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("resourceList", resourceList);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selResourceRole", method = RequestMethod.POST)
	public RetInfoDto selResourceRole(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowRole> roleList = this.powerService.selResourceRole(dto);
			if (roleList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("s", roleList);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_ROLE_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/addUserRole", method = RequestMethod.POST)
	public RetInfoDto addUserRole(@RequestBody PowUserRoleKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.addUserRole(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/delUserRole", method = RequestMethod.POST)
	public RetInfoDto delUserRole(@RequestBody PowUserRoleKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			boolean isOk = this.powerService.delUserRole(dto);
			if (isOk) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			}

		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selUserRole", method = RequestMethod.POST)
	public RetInfoDto selUserRole(@RequestBody PowUserRoleKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowRole> roleList = this.powerService.selUserRole(dto);
			if (roleList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("roleList", roleList);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_USER_ROLE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selRoleUser", method = RequestMethod.POST)
	public RetInfoDto selRoleUser(@RequestBody PowUserRoleKey dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowSysUser> userList = this.powerService.selRoleUser(dto);
			if (userList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("userList", userList);
			} else {
				info.meta.put("code", ConstParm.ERR_NO_POW_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selUserResource", method = RequestMethod.POST)
	public RetInfoDto selUserResource(@RequestBody PowSysUser dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowResource> resourceList = this.powerService.selUserResource(dto);
			if (resourceList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("resourceList", resourceList);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_USER_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_USER_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

	@Transactional
	@RequestMapping(value = "/selResourceUser", method = RequestMethod.POST)
	public RetInfoDto selResourceUser(@RequestBody PowResource dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			dto.setAppId(jwtToken.getApp_id());
			List<PowSysUser> userList = this.powerService.selResourceUser(dto);
			if (userList.size() > 0) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("userList", userList);
			} else {
				info.meta.put("code", ConstParm.ERR_POW_USER_RESOURCE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("code", ConstParm.ERR_POW_USER_RESOURCE);
			info.meta.put("mess", e.getMessage());
		}
		return info;
	}

}